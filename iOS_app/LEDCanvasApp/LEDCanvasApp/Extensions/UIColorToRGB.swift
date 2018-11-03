//
//  UIColorToRGB.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/3/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

extension UIColor {
    
    func getRGB() -> [String: Int]? {
        
        var rgbDict: [String: Int] = ["alpha" : 0, "red" : 0, "blue" :0, "green" : 0]
        
        var red:   CGFloat = 0
        var blue:  CGFloat = 0
        var green: CGFloat = 0
        var alpha: CGFloat = 0
        
        if self.getRed(&red, green: &green, blue: &blue, alpha: &alpha) {
            
            let _red   = Int(red * 255)
            let _blue  = Int(blue * 255)
            let _green = Int(green * 255)
            let _alpha = Int(alpha * 255)
            
            rgbDict["alpha"] = _alpha << 24
            rgbDict["red"]   = _red   << 16
            rgbDict["green"] = _green << 8
            rgbDict["blue"]  = _blue
        }
        
        return rgbDict
    }
}
