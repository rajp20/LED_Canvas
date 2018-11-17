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
    var lastPoint: CGPoint!
    var swiped:    Bool!
    var color = UIColor.black
    var brushWidth: CGFloat = 8.0
    var opacity: CGFloat = 1.0
    
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
    }
    
    @objc func clearContents() {
        tempImage.image = nil
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
        context.setStrokeColor(color.cgColor) //need to figure out UIColor constructor doesn't work with rgb
        
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
        let string = JSONString(point: lastPoint, color: color)
        
        writeValue(data: "start")
        let dataToSend = string.group(of: 20)
        for data in dataToSend {
            writeValue(data: data)
        }
        writeValue(data: "end")
    }
    
    func JSONString(point: CGPoint, color: UIColor) -> String {
        let rgb = color.getRGB()
//        print(rgb?["red"])
//        print(rgb?["green"])
//        print(rgb?["blue"])
        let JSONString = "{\"x\":\(Int(point.x)),\"y\":\(Int(point.y)),\"r\":\(rgb?["red"] ?? 0),\"g\":\(rgb?["green"] ?? 0),\"b\":\(rgb?["blue"] ?? 0)}"
//        let JSONString = "{\"x\":\(Int(point.x)/60),\"y\":\(Int(point.y)/18)}"
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
