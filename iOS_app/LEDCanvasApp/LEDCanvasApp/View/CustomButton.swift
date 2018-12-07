//
//  CustomButton.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/26/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class CustomButton : UIButton {
    
    private var offColor = UIColor(red: 0.37, green: 0.37, blue: 0.37, alpha: 1.0)
    private var onColor  = UIColor(red: 0.57, green: 0.57, blue: 0.57, alpha: 1.0)
    private var offColorFontColor = UIColor(red: 0.2, green: 0.68, blue: 0.47, alpha: 1.0)
    private var onFontColor = UIColor(red: 0.29, green: 1.0, blue: 0.71, alpha: 1.0)
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        layer.cornerRadius = 10
        backgroundColor = offColor
        setTitleColor(offColorFontColor, for: .normal)
    }
    
    func toggleButton() {
        if backgroundColor == onColor {
            backgroundColor = offColor
            setTitleColor(offColorFontColor, for: .normal)
        }
        else {
            backgroundColor = onColor
            setTitleColor(onFontColor, for: .normal)
        }
    }
    
    func backgroundColorOn() {
        backgroundColor = onColor
        setTitleColor(onFontColor, for: .normal)
    }
    
    func backgroundColorOff() {
        backgroundColor = offColor
        setTitleColor(offColorFontColor, for: .normal)
    }
}
