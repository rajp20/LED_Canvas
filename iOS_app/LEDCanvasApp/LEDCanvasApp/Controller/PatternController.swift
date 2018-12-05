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
    @IBOutlet weak var ballPatternButton: CustomButton!
    @IBOutlet weak var ripplePatternButton: CustomButton!
    
    override func viewDidLoad() {
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
    }
    @IBAction func BallPatternClicked(_ sender: Any) {
        
        if !uartVC!.patterns.ballPatternInProgress {
            uartVC!.dataQueue.clearQueue()
            /** This is where the pattern gets hung up without a delay **/
            var count = 0
            while count < 1000 {
                count += 1
            }
            uartVC!.writeValue(data: "tbbon")
        } else { uartVC!.writeValue(data: "tbboff")}
        
        uartVC!.patterns.ballPatternInProgress = !uartVC!.patterns.ballPatternInProgress
        print(uartVC!.patterns.ballPatternInProgress)

    }
    
    @IBAction func RipplePatternClicked(_ sender: Any) {
    }
}
