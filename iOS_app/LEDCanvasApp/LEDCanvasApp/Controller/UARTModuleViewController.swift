//
//  UARTModuleViewController.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 9/14/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import Foundation
import UIKit
import CoreBluetooth

/**
 * View controller used for drawing and sending coordinate and color data to the microcontroller.
 */
class UARTModuleViewController: UIViewController, CBPeripheralManagerDelegate {
    
    // Bluetooth variables
    var peripheralManager: CBPeripheralManager?
    var peripheral: CBPeripheral!
    var idleState : Bool!
    
    // Variable used to know if we should send a reset to arduino
    var shouldReset : Bool!
    var resetButtons : Bool!
    var patternInProgress : Bool!
    
    // Drawing variables
    var lastPoint  : CGPoint!
    var swiped     : Bool!
    var brushWidth : CGFloat = 10.0
    var opacity    : CGFloat = 1.0
    var color = UIColor(red: 50/255.0, green: 245/255.0, blue: 176/255.0, alpha: 1)
    
    //Queues for drawing and data to be transmitted
    private var queue        : Queue<Line>!
    var dataQueue            : Queue<CGPoint>!
    private var pixelTimer   : Timer!
    private var timeInterval : TimeInterval = 0.08
    private var firstLoad    : Bool!
    private var prevPixel    : CGPoint!
    
    @IBOutlet weak var tempImage: UIImageView!
    
    // Struct used to save the state of a running pattern from PatternController
    // after the reference to the PatternController dissapears.
    public struct Patterns {
        var ballPatternInProgress   = false
        var ripplePatternInProgress = false
        var acidPatternInProgress   = false
        var lifePatternInProgress   = false
        
        mutating func resetPatterns() {
            ballPatternInProgress   = false
            ripplePatternInProgress = false
            acidPatternInProgress   = false
            lifePatternInProgress   = false
        }
    }
    
    var patterns : Patterns!
    
    /**
    * Initialize necessary subviews and data members once the view has loaded.
    */
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
        self.navigationItem.rightBarButtonItem?.tintColor = UIColor(red: 0.29, green: 1.0, blue: 0.71, alpha: 1.0)
        
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
        lastPoint = CGPoint.zero
        prevPixel = CGPoint.zero
        swiped    = false
        firstLoad = false
        idleState = true
        shouldReset = false
        resetButtons = false
        patternInProgress = false
        
        queue      = Queue<Line>()
        dataQueue  = Queue<CGPoint>()
        pixelTimer = Timer()
        patterns   = Patterns()
        startPixelTimer()
        setupMenuBar()
    }
    
    /**
    * Once the view has been loaded, this function is triggered by UIView protocol. This function sends
    * the defualt rgb color of the brush to the microcontroller.
    */
    override func viewDidAppear(_ animated: Bool) {
        if(!firstLoad) {
            firstLoad = true
            writeValue(data: colorString())
        }
    }
    
    // Custom menu bar used for selecting patterns, color, and resetting view.
    lazy var menuBar: MenuBar = {
        let mb = MenuBar()
        mb.delegate = self
        return mb
    }()
    
    // Abstraction function used to setup the menuBar, this is called from the viewDidLoad function.
    private func setupMenuBar() {
        view.addSubview(menuBar)
        
        menuBar.translatesAutoresizingMaskIntoConstraints = false // this will make your constraint working
        menuBar.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor).isActive = true // every constraint must be enabled.
        menuBar.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor).isActive = true
        menuBar.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor, constant: -10).isActive = true
        menuBar.heightAnchor.constraint(equalToConstant: 120).isActive = true
    }
    
    /**
    * This function is a timer that is used to fade out the pixel values on the view at a given interval.  The timer calls updatePixels() at
    * each interval.
    */
    private func startPixelTimer() {
        guard pixelTimer == nil else { return }
        pixelTimer = Timer.scheduledTimer(timeInterval: timeInterval, target: self, selector: #selector(updatePixels), userInfo: nil, repeats: true)
    }
    
    /**
    * This function is used to stop the pixelTimer if ever needed. (Currently not ever used.)
    */
    private func stopPixelTimer() {
        guard pixelTimer != nil else { return }
        pixelTimer?.invalidate()
        pixelTimer = nil
    }
    
    /**
    * This function is called when the timer hits a given interval. The view used as the drawing canvas is cleared so that
    * it can be drawn to cleanly.  After that, the function loops through the coordinates that have already been drawn and
    * decreases the alpha value. Once the alpha value reaches zero or lower, the pixel is popped off the queue and removed
    * from the canvas.
    */
    @objc private func updatePixels() {
        tempImage.image = nil
        queue.updateQueue(weight: 0.1)
        let q = queue.list()
        for line in q! {
            
            UIGraphicsBeginImageContext(tempImage.frame.size)
            guard let context = UIGraphicsGetCurrentContext() else {
                return
            }
            
            tempImage.image?.draw(in: tempImage.bounds)
            
            context.move(to: line.line["from"]!)
            context.addLine(to: line.line["to"]!)
            
            context.setLineCap(.round)
            context.setBlendMode(.copy)
            context.setLineWidth(brushWidth)
            var rgb = color.getRGB()
            let newColor = UIColor(red: rgb!["red"]!/255.0, green: rgb!["green"]!/255.0, blue: rgb!["blue"]!/255.0, alpha: line.alpha)
            context.setStrokeColor(newColor.cgColor)
            
            context.strokePath()
            
            tempImage.image = UIGraphicsGetImageFromCurrentImageContext()
            tempImage.alpha = opacity
            UIGraphicsEndImageContext()
        }
    }
    
    /**
    * clearContents() clears the UIImageView used to draw and it clears the dataQueue to prevent sending unnecessary data to the
    * microcontroller.  A command is transmitted to the microntroller to notify it that everything has been cleared. The app then
    * goes into "idle" state.
    */
    public func clearContents() {
        tempImage.image = nil
        queue.clearQueue()
        dataQueue.clearQueue()
        if (idleState == true){
            writeValue(data: "0")
            return
        }
        shouldReset = true
    }
    
    /**
    * Detect touch events to begin drawing. This function detects only one given touch.
    */
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        swiped = false
        lastPoint = touch.location(in: tempImage)
        prevPixel = lastPoint
        prevPixel.x /= 17
        prevPixel.y /= 28
    }
    
    /**
    * Detects when a user is moving their finger acrossed the screen to draw.  The function ensures that ther is no pattern running and then begins
    * to draw a on the UIView.
    */
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        
        swiped = true
        let currentPoint = touch.location(in: tempImage)
        
        if dataQueue.count() > 20 {
            dataQueue.clearQueue()
            writeValue(data: "p5")
        }
        
        if !patternInProgress {
            
            if tempImage.bounds.contains(lastPoint) {
                
                drawLine(from: lastPoint, to: currentPoint)
                queue.enqueue(Line(lineAt: ["from": lastPoint, "to": currentPoint], alphaValue: 1.0))
                
                lastPoint = currentPoint
                
                if idleState {
                    idleState = false
                    
                    var currPixel = currentPoint
                    currPixel.x /= 17
                    currPixel.y /= 28
                    
                    let coordinate = coordinateString(point: currPixel)
                    
                    // Check to see if a reset needs to be sent, as it has priority over everything
                    if (shouldReset == true){
                        writeValue(data: "0")
                        shouldReset = false
                    }
                    else {
                        if Int(prevPixel.x) != Int(currentPoint.x / 17) || Int(prevPixel.y) != Int(currentPoint.y / 28) {
                        writeValue(data: coordinate)
                        }
                        prevPixel = currPixel
                    }
                }
                    
                else {
                    if (Int(prevPixel.x) != Int(currentPoint.x / 17)) || (Int(prevPixel.y) != Int(currentPoint.y / 28)) {
                        
                        var currPixel = currentPoint
                        currPixel.x /= 17
                        currPixel.y /= 28
                        
                        dataQueue.enqueue(prevPixel)
                        prevPixel = currPixel
                    }
                }
            }
        }
    }
    
    /**
    * Function used to do the actual drawing to the UIImageView using Graphics contexts.
    */
    func drawLine(from fromPoint: CGPoint, to toPoint: CGPoint) {
        
        UIGraphicsBeginImageContext(tempImage.frame.size)
        guard let context = UIGraphicsGetCurrentContext() else {
            return
        }
        tempImage.image?.draw(in: tempImage.bounds)
        
        context.move(to: fromPoint)
        context.addLine(to: toPoint)
        
        context.setLineCap(.round)
        context.setBlendMode(.normal)
        context.setLineWidth(brushWidth)
        context.setStrokeColor(color.cgColor)
        
        context.strokePath()
        
        tempImage.image = UIGraphicsGetImageFromCurrentImageContext()
        tempImage.alpha = opacity
        UIGraphicsEndImageContext()
    }
    
    /**
    * Formats a string with the current coordinate (point) that needs to be sent to the microcontroller.
    */
    func coordinateString(point: CGPoint) -> String {
        let coordinateString = "x,\(Int(point.x)),\(Int(point.y));"
        return coordinateString
    }
    
    /**
     * Formats a string with the current color that needs to be sent to the microcontroller.
     */
    func colorString() -> String{
        let rgb = color.getRGB()
        let colorString = "c,\(Int(rgb?["red"] ?? 0)),\(Int(rgb?["green"] ?? 0)),\(Int(rgb?["blue"] ?? 0));"
        return colorString
    }
    
    func peripheralManagerDidUpdateState(_ peripheral: CBPeripheralManager) {
        if peripheral.state == .poweredOn {
            return
        }
        print("Peripheral manager is running")
    }
    
    // Write function used to send a string to the microcontroller.
    func writeValue(data: String){
        
        print("Sending: " + data)
        
        let valueString = (data as NSString).data(using: String.Encoding.utf8.rawValue)
        //change the "data" to valueString
        if let blePeripheral = blePeripheral {
            if let txCharacteristic = txCharacteristic {
                blePeripheral.writeValue(valueString!, for: txCharacteristic, type: CBCharacteristicWriteType.withResponse)
            }
        }
    }

    func writeCharacteristic(val: Int8){
        var val = val
        let ns = NSData(bytes: &val, length: MemoryLayout<Int8>.size)
        blePeripheral!.writeValue(ns as Data, for: txCharacteristic!, type: CBCharacteristicWriteType.withResponse)
    }
}
