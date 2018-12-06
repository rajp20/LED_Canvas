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
    @IBOutlet weak var acidPatternButton: CustomButton!
    @IBOutlet weak var lifePatternButton: CustomButton!
    
    var buttons : [CustomButton]!
    
    override func viewDidLoad() {
        self.view.backgroundColor = UIColor(red: 0.29, green: 0.29, blue: 0.29, alpha: 1.0)
        buttons = [ballPatternButton, ripplePatternButton]
    }

    @IBAction func BallPatternClicked(_ sender: Any) {
        
        if !uartVC!.patterns.ballPatternInProgress {
            uartVC!.dataQueue.clearQueue()
            
            for button in buttons {
                if button == sender as? CustomButton {
                    continue
                }
                button.isUserInteractionEnabled = false
            }
            var count = 0
            while count < 1000 {
                count += 1
            }
            uartVC!.writeValue(data: "p11")
        }
        else {
            uartVC!.writeValue(data: "p10")
            
            for button in buttons {
                button.isUserInteractionEnabled = true
            }
        }
        
        uartVC!.patterns.ballPatternInProgress = !uartVC!.patterns.ballPatternInProgress
    }
    
    @IBAction func RipplePatternClicked(_ sender: Any) {
        //rip1 on, rip0 off
        if !uartVC!.patterns.ripplePatternInProgress {
            uartVC!.dataQueue.clearQueue()
            
            for button in buttons {
                if button == sender as? CustomButton {
                    continue
                }
                button.isUserInteractionEnabled = false
            }
            var count = 0
            while count < 1000 {
                count += 1
            }
            uartVC!.writeValue(data: "p21")
        }
        else {
            uartVC!.writeValue(data: "p20")
            
            for button in buttons {
                button.isUserInteractionEnabled = true
            }
        }
        
        uartVC!.patterns.ripplePatternInProgress = !uartVC!.patterns.ripplePatternInProgress
    }
    
    @IBAction func AcidPatternClicked(_ sender: Any) {
        
        if !uartVC!.patterns.acidPatternInProgress {
            uartVC!.dataQueue.clearQueue()
            
            for button in buttons {
                if button == sender as? CustomButton {
                    continue
                }
                button.isUserInteractionEnabled = false
            }
            var count = 0
            while count < 1000 {
                count += 1
            }
            uartVC!.writeValue(data: "p31")
        }
        else {
            uartVC!.writeValue(data: "p30")
            
            for button in buttons {
                button.isUserInteractionEnabled = true
            }
        }
        
        uartVC!.patterns.acidPatternInProgress = !uartVC!.patterns.acidPatternInProgress
    }
    
    @IBAction func LifePatternClicked(_ sender: Any) {
        
        if !uartVC!.patterns.lifePatternInProgress {
            uartVC!.dataQueue.clearQueue()
            
            for button in buttons {
                if button == sender as? CustomButton {
                    continue
                }
                button.isUserInteractionEnabled = false
            }
            var count = 0
            while count < 1000 {
                count += 1
            }
            uartVC!.writeValue(data: "p41")
        }
        else {
            uartVC!.writeValue(data: "p40")
            
            for button in buttons {
                button.isUserInteractionEnabled = true
            }
        }
        
        uartVC!.patterns.lifePatternInProgress = !uartVC!.patterns.lifePatternInProgress
    }
}
