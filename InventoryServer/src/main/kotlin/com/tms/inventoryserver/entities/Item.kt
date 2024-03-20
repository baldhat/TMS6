package com.tms.inventoryserver.entities

import jakarta.persistence.Entity
import jakarta.persistence.Id

@Entity
class Item(
        var name: String,
        var isPresent: Boolean,
        @Id
        val id: String,
        var scannerId: String = "0",
        var active: Boolean = true
)