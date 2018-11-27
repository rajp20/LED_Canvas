//
//  ColorController.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/25/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class ColorController : UIViewController {
    
    @IBOutlet weak var redSlider:   UISlider!
    @IBOutlet weak var greenSlider: UISlider!
    @IBOutlet weak var blueSlider:  UISlider!
    
    @IBOutlet weak var redLabel:      UILabel!
    @IBOutlet weak var greenLabel:    UILabel!
    @IBOutlet weak var blueLabel:     UILabel!
    @IBOutlet weak var colorSelected: UILabel!
    
    @IBOutlet weak var applyButton: CustomButton!
    
    private var red:   Float!
    private var green: Float!
    private var blue:  Float!
    
    weak var uartVC: UARTModuleViewController?
    
    override func viewDidLoad() {
       initializeLabelsAndSliders()
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
    }
    
    private func initializeLabelsAndSliders() {
        let color = uartVC?.color.getRGB()
        red   = Float(color!["red"]!)
        green = Float(color!["green"]!)
        blue  = Float(color!["blue"]!)
        
        colorSelected.backgroundColor = uartVC?.color
        colorSelected.layer.cornerRadius = 10
        colorSelected.clipsToBounds = true
        
        redLabel.text   = "Red: \(Int(red))"
        greenLabel.text = "Green: \(Int(green))"
        blueLabel.text  = "Blue: \(Int(blue))"
        
        redLabel.textColor   = UIColor.white
        greenLabel.textColor = UIColor.white
        blueLabel.textColor  = UIColor.white
        
        redLabel.font   = UIFont(name: "Avenir", size: 24)
        greenLabel.font = UIFont(name: "Avenir", size: 24)
        blueLabel.font  = UIFont(name: "Avenir", size: 24)
        
        redSlider.minimumValue   = 0.0
        greenSlider.minimumValue = 0.0
        blueSlider.minimumValue  = 0.0
        
        redSlider.maximumValue   = 255.0
        greenSlider.maximumValue = 255.0
        blueSlider.maximumValue  = 255.0
        
        redSlider.value   = red
        greenSlider.value = green
        blueSlider.value  = blue
    }
    
    
    @IBAction func RedSliderAction(_ sender: UISlider) {
        ChangeColor()
    }

    @IBAction func GreenSliderAction(_ sender: UISlider) {
        ChangeColor()
    }
    
    @IBAction func BlueSliderAction(_ sender: UISlider) {
        ChangeColor()
    }
    
    @IBAction func ApplyColorToBrush(_ sender: UIButton) {
        uartVC!.color = colorSelected.backgroundColor!
    }
    
    private func ChangeColorSelected() {
        colorSelected.backgroundColor = UIColor(red: CGFloat(red / 255), green: CGFloat(green / 255), blue: CGFloat(blue / 255), alpha: 1.0)
        UpdateLabels()
    }
    
    private func ChangeColor() {
        red   = redSlider.value
        green = greenSlider.value
        blue  = blueSlider.value
        
        ChangeColorSelected()
    }
    
    private func UpdateLabels() {
        redLabel.text   = "Red:\(Int(redSlider.value))"
        greenLabel.text = "Green:\(Int(greenSlider.value))"
        blueLabel.text  = "Blue:\(Int(blueSlider.value))"
    }
}
