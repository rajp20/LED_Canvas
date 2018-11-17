//
//  MenuCell.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/16/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class MenuCell: UICollectionViewCell {
    
    @IBOutlet weak var cellImage: UIImageView!
    @IBOutlet weak var cellLabel: UILabel!
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self.backgroundColor = UIColor.blue
        self.layer.cornerRadius = 10.0
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
