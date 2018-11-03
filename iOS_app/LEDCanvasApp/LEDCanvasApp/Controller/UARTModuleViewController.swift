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
    var lastPoint: CGPoint!
    var swiped:    Bool!
    var color = UIColor.black
    var brushWidth: CGFloat = 10.0
    var opacity: CGFloat = 1.0
    
    // Two UIImageViews for drawing on
    @IBOutlet weak var mainImage: UIImageView!
    @IBOutlet weak var tempImage: UIImageView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        peripheralManager = CBPeripheralManager(delegate: self, queue: nil)
        lastPoint = CGPoint.zero
        swiped   = false
    }
    
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
        let str = "!L" + "\(Int(lastPoint.x))," + "\(Int(lastPoint.y))"
//        print(str)
        writeValue(data: str)
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
    
//    func writeValue(point: CGPoint) {
//
//    }
}
