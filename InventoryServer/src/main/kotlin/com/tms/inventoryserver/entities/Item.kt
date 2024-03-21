package com.tms.inventoryserver.entities

import jakarta.persistence.Entity
import jakarta.persistence.Id
import java.util.*
import kotlin.random.Random

@Entity
class Item(
        var name: String,
        var isPresent: Boolean,
        @Id val id: String,
        var scannerId: String = "0",
        var expiresAt: Long = Random.nextLong(Date().time - 15000000000, Date().time + 70000000000),
        var active: Boolean = true
)