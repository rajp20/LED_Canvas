//
//  PatternController.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/25/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class PatternController : UIViewController {
    
    weak var uartVC : UARTModuleViewController?
    override func viewDidLoad() {
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
    }
}
