//
//  Pixel.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/18/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

public class Pixel {
    var point : CGPoint!
    var alpha : CGFloat!
    
    public init(pointAt: CGPoint, alphaValue: CGFloat) {
        point = pointAt
        alpha = alphaValue
    }
    
    public func getPoint() -> CGPoint {
        return point
    }
    
    public func getAlpha() -> CGFloat {
        return alpha
    }
    
    public func setAlpha(opacity: CGFloat) {
        alpha = opacity
    }
}
