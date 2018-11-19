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

class UARTModuleViewController: UIViewController, CBPeripheralManagerDelegate, UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout {
    
    var data = [MenuCell(), MenuCell(), MenuCell(), MenuCell()]
    var peripheralManager: CBPeripheralManager?
    var peripheral: CBPeripheral!
    
    // Drawing variables
    var lastPoint : CGPoint!
    var swiped    : Bool!
    var color = UIColor(red: 50/255.0, green: 245/255.0, blue: 176/255.0, alpha: 1)
    var brushWidth : CGFloat = 8.0
    var opacity    : CGFloat = 1.0
    
    private var queue        : Queue<Pixel>!
    private var pixelTimer   : Timer!
    private var timeInterval : TimeInterval = 0.1
    
    // Two UIImageViews for drawing on
//    @IBOutlet weak var mainImage: UIImageView!
    @IBOutlet weak var tempImage: UIImageView!
    @IBOutlet weak var menuBar: UICollectionView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.navigationItem.rightBarButtonItem = UIBarButtonItem(title: "Clear", style: .done, target: self, action: #selector(clearContents))
        
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
        lastPoint = CGPoint.zero
        swiped   = false
        
        menuBar.dataSource = self
        menuBar.delegate   = self
        menuBar.register(MenuCell().classForCoder, forCellWithReuseIdentifier: "MenuCell")
        menuBar.backgroundColor = UIColor.gray
//        menuBar.collectionViewLayout = UICollectionViewFlowLayout()
        
        queue = Queue<Pixel>()
        pixelTimer = Timer()
        startTimer()
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
        queue.updateQueue(weight: 0.1)
        let q = queue.list()
        for pixel in q! {
            // this is where the tempImage variable will be updated.  The timer calls this function
            // at a set interval and updates the alpha values of the pixels already drawn to specific
            // points on the image. The problem is that the drawLine method uses core graphics to draw
            // a line between two points but the lines themselves are not being stored in the queue.
            // Will need to check if queue will need to be updated with lines instead of queues.
            UIGraphicsBeginImageContext(view.frame.size)
            guard let context = UIGraphicsGetCurrentContext() else {
                return
            }
            
            tempImage.image?.draw(in: view.bounds)
            
            context.move(to: pixel.point)
            context.addLine(to: pixel.point)
            
            context.setLineCap(.round)
            context.setBlendMode(.normal)
            context.setLineWidth(brushWidth)
            var rgb = color.getRGB()
            let newColor = UIColor(red: rgb!["red"]!/255.0, green: rgb!["green"]!/255.0, blue: rgb!["blue"]!/255.0, alpha: pixel.alpha)
            context.setStrokeColor(newColor.cgColor)
            
            context.strokePath()
            
            tempImage.image = UIGraphicsGetImageFromCurrentImageContext()
            tempImage.alpha = opacity // update alpha value
            UIGraphicsEndImageContext()
        }
    }
    
    @objc private func clearContents() {
        tempImage.image = nil
    }
    
    //Detect touch events to begin drawing
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        swiped = false
        lastPoint = touch.location(in: view)
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        
        // 6
        swiped = true
        let currentPoint = touch.location(in: view)
        drawLine(from: lastPoint, to: currentPoint)
        
        // Add lastPoint to queue
        queue.enqueue(Pixel(pointAt: CGPoint(x: lastPoint.x, y: lastPoint.y), alphaValue: 1.0))
        
        // 7
        lastPoint = currentPoint
        let string = JSONString(point: lastPoint, color: color)
        
        writeValue(data: "start")
        let dataToSend = string.group(of: 20)
        for data in dataToSend {
            writeValue(data: data)
        }
        writeValue(data: "end")
    }
    
    func drawLine(from fromPoint: CGPoint, to toPoint: CGPoint) {
        // 1
        UIGraphicsBeginImageContext(view.frame.size)
        guard let context = UIGraphicsGetCurrentContext() else {
            return
        }
        tempImage.image?.draw(in: view.bounds)
        
        // 2
        context.move(to: fromPoint)
        context.addLine(to: toPoint)
        
        // 3
        context.setLineCap(.round)
        context.setBlendMode(.normal)
        context.setLineWidth(brushWidth)
        context.setStrokeColor(color.cgColor)
        
        // 4
        context.strokePath()
        
        // 5
        tempImage.image = UIGraphicsGetImageFromCurrentImageContext()
        tempImage.alpha = opacity
        UIGraphicsEndImageContext()
    }
    
    func JSONString(point: CGPoint, color: UIColor) -> String {
        let rgb = color.getRGB()
        let JSONString = "{\"x\":\(Int(point.x)),\"y\":\(Int(point.y)),\"r\":\(Int(rgb?["red"] ?? 0)),\"g\":\(Int(rgb?["green"] ?? 0)),\"b\":\(Int(rgb?["blue"] ?? 0))}"
        return JSONString
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
    
    
    /********** UICollectionView protocols *********/
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return 3
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "MenuCell", for: indexPath) as! MenuCell
        //        cell.configure(with: data[indexPath.row])
        return cell
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        return CGSize(width: collectionView.frame.width / 3.3, height: collectionView.frame.height)
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumLineSpacingForSectionAt section: Int) -> CGFloat {
        return 0
    }
}
