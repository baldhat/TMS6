package com.tms.inventoryserver.entities

import jakarta.persistence.Entity
import jakarta.persistence.Id

@Entity
class Item(
        var name: String,
        var isPresent: Boolean,
        @Id
        val id: String,
)