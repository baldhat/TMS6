package com.tms.inventoryserver.controllers

import com.tms.inventoryserver.LoadDatabase
import com.tms.inventoryserver.entities.Item
import com.tms.inventoryserver.repositories.ItemRepository
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.PathVariable
import org.springframework.web.bind.annotation.PostMapping
import org.springframework.web.bind.annotation.RequestBody
import org.springframework.web.bind.annotation.RestController
import kotlin.random.Random

@RestController
class ItemListController(
        private val repository: ItemRepository) {

    @GetMapping("/items")
    fun getKnownItems(): List<Item> {
        val items = repository.findAllByOrderByIsPresentAsc()
        return items
    }

    @PostMapping("/scanners/{id}/present")
    fun setPresentItems(@PathVariable id: String, @RequestBody presentIds: List<String> ): Boolean {
        log.info("Received set present items")
        repository.findByScannerId(id).forEach {
            it.isPresent = presentIds.contains(it.id)
            repository.save(it)
        }
        return true;
    }

    @PostMapping("/items/add")
    fun addItem(@RequestBody item: Item): Boolean {
        log.info("Adding new item: $item")
        repository.save(item)
        return true
    }

    companion object {
        private val log: Logger = LoggerFactory.getLogger(LoadDatabase::class.java)
    }
}