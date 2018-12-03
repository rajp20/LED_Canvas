//
//  CustomButton.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/26/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class CustomButton : UIButton {
    
    override func awakeFromNib() {
        super.awakeFromNib()
        
        layer.cornerRadius = 10
        backgroundColor = UIColor(red: 0.37, green: 0.37, blue: 0.37, alpha: 1.0)
        let fontColor = UIColor(red: 0.29, green: 1.0, blue: 0.71, alpha: 1.0)
        setTitleColor(fontColor, for: .normal)
    }
}
