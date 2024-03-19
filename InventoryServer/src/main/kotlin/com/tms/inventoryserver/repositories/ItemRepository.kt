package com.tms.inventoryserver.repositories

import com.tms.inventoryserver.entities.Item
import org.springframework.data.jpa.repository.JpaRepository

interface ItemRepository : JpaRepository<Item, Long> {
    fun findByName(name: String): List<Item>

    fun findAllByOrderByIsPresentAsc(): List<Item>
    fun findByScannerId(id: String): List<Item>
}