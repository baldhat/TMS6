package com.tms.inventoryserver.repositories

import com.tms.inventoryserver.entities.Item
import org.springframework.data.jpa.repository.JpaRepository

interface ItemRepository : JpaRepository<Item, Long> {

    fun findByActiveOrderByIsPresentAsc(active: Boolean): List<Item>
    fun findById(id: String): Item
    fun findByScannerId(id: String): List<Item>
}