//
//  PeripheralCell.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 9/17/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

class PeripheralCell: UITableViewCell {

    @IBOutlet weak var peripheralLabel : UILabel!
    @IBOutlet weak var rssiLabel :       UILabel!
    
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
