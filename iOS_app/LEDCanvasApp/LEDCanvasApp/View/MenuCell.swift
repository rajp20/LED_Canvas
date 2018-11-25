//
//  MenuCell.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/16/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class MenuCell: UICollectionViewCell {
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self.backgroundColor = UIColor(red: 0.3, green: 0.3, blue: 0.3, alpha: 1.0)
        self.layer.cornerRadius = 10.0
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
