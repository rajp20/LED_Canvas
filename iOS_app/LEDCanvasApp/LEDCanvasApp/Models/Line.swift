//
//  Pixel.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/18/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

public class Line {
    var alpha : CGFloat!
    var line: [String : CGPoint]
    public init(lineAt: [String : CGPoint], alphaValue: CGFloat) {
        alpha = alphaValue
        line = lineAt
    }
    
    public func getLine() -> [String : CGPoint] {
        return line
    }
    
    public func getAlpha() -> CGFloat {
        return alpha
    }
    
    public func setAlpha(opacity: CGFloat) {
        alpha = opacity
    }
}
