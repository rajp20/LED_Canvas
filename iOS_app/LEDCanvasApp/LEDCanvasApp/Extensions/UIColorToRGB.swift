//
//  UIColorToRGB.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/3/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

extension UIColor {
    
    func getRGB() -> [String: CGFloat]? {
        
        var rgbDict: [String: CGFloat] = ["alpha" : 0.0, "red" : 0.0, "blue" : 0.0, "green" : 0.0]
        
        var red:   CGFloat = 0
        var blue:  CGFloat = 0
        var green: CGFloat = 0
        var alpha: CGFloat = 0
        
        if self.getRed(&red, green: &green, blue: &blue, alpha: &alpha) {
            
            let _red   = red
            let _blue  = blue
            let _green = green
            let _alpha = alpha
            
            rgbDict["alpha"] = _alpha
            rgbDict["red"]   = _red
            rgbDict["green"] = _green
            rgbDict["blue"]  = _blue
        }
        
        return rgbDict
    }
}
