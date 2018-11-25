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
    
    private var red:   Float!
    private var green: Float!
    private var blue:  Float!
    
    override func viewDidLoad() {
        //any code on initialization
        red   = 0.0
        green = 0.0
        blue  = 0.0
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
    
    private func ChangeColorSelected() {
        colorSelected.backgroundColor = UIColor(red: CGFloat(red), green: CGFloat(green), blue: CGFloat(blue), alpha: 1.0)
        UpdateLabels()
    }
    
    private func ChangeColor() {
        red   = redSlider.value
        green = greenSlider.value
        blue  = blueSlider.value
        
        ChangeColorSelected()
    }
    
    private func UpdateLabels() {
        redLabel.text   = "Red:\(Int(redSlider.value * 255))"
        greenLabel.text = "Green:\(Int(greenSlider.value * 255))"
        blueLabel.text  = "Blue:\(Int(blueSlider.value * 255))"
    }
}
