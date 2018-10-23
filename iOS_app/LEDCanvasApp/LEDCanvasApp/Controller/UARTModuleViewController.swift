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
    var toggleLED:  String!
    
    // Drawing variables
    var lastPoint: CGPoint!
    var swiped:    Bool!
    var color = UIColor.black
    var brushWidth: CGFloat = 10.0
    var opacity: CGFloat = 1.0
    
    // Two UIImageViews for drawing on
    @IBOutlet weak var mainImage: UIImageView!
    @IBOutlet weak var tempImage: UIImageView!
    // This is the outlet for the send data button
//    @IBOutlet weak var redButton:    UIButton!
//    @IBOutlet weak var greenButton:  UIButton!
//    @IBOutlet weak var blueButton:   UIButton!
//    @IBOutlet weak var offButton: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
        toggleLED = "!B11"
        lastPoint = CGPoint.zero
        swiped   = false
    }
    
//    @IBAction func ledOff(_ sender: UIButton) {
//        print("writing value: !B4")
//        writeValue(data: "!B4")
//    }
//
//    @IBAction func blueLED(_ sender: UIButton) {
//        print("writing value: !B3")
//        writeValue(data: "!B3")
//    }
//
//    @IBAction func greenLED(_ sender: UIButton) {
//        print("writing value: !B2")
//        writeValue(data: "!B2")
//    }
//    /**
//     * This action is connected to the send data button and will toggle the leds. this is red led
//     **/
//    @IBAction func ledToggleAction(_ sender: UIButton) {
////        if toggleLED == "off" {
////            toggleLED = "on"
////        }
////        else{ toggleLED = "off" }
//        print("writing value: !B1")
//        writeValue(data: "!B1")
//    }
    
    //Detect touch events to begin drawing
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        swiped = false
        lastPoint = touch.location(in: view)
//        print(lastPoint)
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
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first else {
            return
        }
        
        // 6
        swiped = true
        let currentPoint = touch.location(in: view)
        drawLine(from: lastPoint, to: currentPoint)
        
        // 7
        lastPoint = currentPoint
        print(lastPoint)
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
        if let blePeripheral = blePeripheral{
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
