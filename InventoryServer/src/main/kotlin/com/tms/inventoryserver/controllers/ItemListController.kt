package com.tms.inventoryserver.controllers

import com.tms.inventoryserver.LoadDatabase
import com.tms.inventoryserver.entities.Item
import com.tms.inventoryserver.repositories.ItemRepository
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.web.bind.annotation.GetMapping
import org.springframework.web.bind.annotation.PostMapping
import org.springframework.web.bind.annotation.RequestBody
import org.springframework.web.bind.annotation.RestController
import kotlin.random.Random

@RestController
class ItemListController(
        private val repository: ItemRepository) {

    @GetMapping("/items")
    fun getKnownItems(): List<Item> {
        println("Retrieving all known items...")
        return repository.findAll()
    }

    @PostMapping("/items")
    fun setPresentItems(@RequestBody presentIds: List<String> ): Boolean {
        log.info("Setting currently present items")
        repository.findAll().forEach {
            it.isPresent = presentIds.contains(it.id)
            repository.save(it)
        }
        return true;
    }

    @PostMapping("/items/add")
    fun addItem(): Boolean {
        log.info("Adding random item")
        val e = Item("Gauze Dressing pad", false, "00000" +
                Random.nextInt(0, 99999).toString().padStart(5, '0'))
        repository.save(e)
        return true
    }

    companion object {
        private val log: Logger = LoggerFactory.getLogger(LoadDatabase::class.java)
    }
}