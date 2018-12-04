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

class UARTModuleViewController: UIViewController, CBPeripheralManagerDelegate {
    
    var peripheralManager: CBPeripheralManager?
    var peripheral: CBPeripheral!
    
    // Drawing variables
    var lastPoint  : CGPoint!
    var swiped     : Bool!
    var brushWidth : CGFloat = 10.0
    var opacity    : CGFloat = 1.0
    var color = UIColor(red: 50/255.0, green: 245/255.0, blue: 176/255.0, alpha: 1)
    
    private var queue        : Queue<Line>!
    private var pixelTimer   : Timer!
    private var timeInterval : TimeInterval = 0.08
    private var firstLoad    : Bool!
    private var prevPixel    : CGPoint!
    
//    @IBOutlet weak var mainImage: UIImageView!
    @IBOutlet weak var tempImage: UIImageView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
        self.navigationItem.rightBarButtonItem?.tintColor = UIColor(red: 0.29, green: 1.0, blue: 0.71, alpha: 1.0)
        
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
        lastPoint = CGPoint.zero
        prevPixel = CGPoint.zero
        swiped    = false
        firstLoad = false
        
        queue = Queue<Line>()
        pixelTimer = Timer()
        startTimer()
        setupMenuBar()
    }
    
    override func viewDidAppear(_ animated: Bool) {
        if(!firstLoad) {
            firstLoad = true
            print("***CONNECTED***")
            writeValue(data: "connected")
            writeValue(data: colorString())
        }
    }
    
    lazy var menuBar: MenuBar = {
        let mb = MenuBar()
        mb.delegate = self
        return mb
    }()
    
    private func setupMenuBar() {
        view.addSubview(menuBar)
        
        menuBar.translatesAutoresizingMaskIntoConstraints = false // this will make your constraint working
        menuBar.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor).isActive = true // every constraint must be enabled.
        menuBar.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor).isActive = true
        menuBar.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor).isActive = true
        menuBar.heightAnchor.constraint(equalToConstant: 80).isActive = true
    }
    
    private func startTimer() {
        guard pixelTimer == nil else { return }
        
        pixelTimer = Timer.scheduledTimer(timeInterval: timeInterval, target: self, selector: #selector(updatePixels), userInfo: nil, repeats: true)
    }
    
    private func stopTimer() {
        guard pixelTimer != nil else { return }
        pixelTimer?.invalidate()
        pixelTimer = nil
    }
    
    @objc private func updatePixels() {
        tempImage.image = nil
        queue.updateQueue(weight: 0.1)
        let q = queue.list()
        for line in q! {
            
            UIGraphicsBeginImageContext(tempImage.frame.size)
            guard let context = UIGraphicsGetCurrentContext() else {
                return
            }
            
            // change back to view.bounds
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
    
    public func clearContents() {
        tempImage.image = nil
        queue.clearQueue()
    }
    
    //Detect touch events to begin drawing
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        swiped = false
        lastPoint = touch.location(in: tempImage)
        prevPixel = lastPoint
        prevPixel.x /= 60
        prevPixel.y /= 27.7
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        
        swiped = true
        let currentPoint = touch.location(in: tempImage)
        
        drawLine(from: lastPoint, to: currentPoint)
        
        // Add lastPoint to queue
        queue.enqueue(Line(lineAt: ["from": lastPoint, "to": currentPoint], alphaValue: 1.0))
        
        lastPoint = currentPoint
        
//        prevPixel = lastPoint
//        prevPixel.x /= 60
//        prevPixel.y /= 27
        
        if ((Int(prevPixel.x) != Int(currentPoint.x / 60)) || (Int(prevPixel.y) != Int(currentPoint.y / 27))) {
            let string = coordinateString(point: prevPixel)
            let dataToSend = string.group(of: 20)
            print("yesyes")
            for data in dataToSend {
                writeValue(data: data)
            }
        }
    }
    
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
    
    func coordinateString(point: CGPoint) -> String {
//        let rgb = color.getRGB()
//        let coordinateString = "{\"x\":\(Int(point.x)),\"y\":\(Int(point.y)),\"r\":\(Int(rgb?["red"] ?? 0)),\"g\":\(Int(rgb?["green"] ?? 0)),\"b\":\(Int(rgb?["blue"] ?? 0))}"
        let coordinateString = "xyz,\(Int(point.x)),\(Int(point.y)),1,"
        return coordinateString
    }
    
    func colorString() -> String{
        let rgb = color.getRGB()
        let colorString = "rgb,\(Int(rgb?["red"] ?? 0)),\(Int(rgb?["green"] ?? 0)),\(Int(rgb?["blue"] ?? 0)),"
        return colorString
    }
    
    func peripheralManagerDidUpdateState(_ peripheral: CBPeripheralManager) {
        if peripheral.state == .poweredOn {
            return
        }
        print("Peripheral manager is running")
    }
    
    // Write functions
    func writeValue(data: String){
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
