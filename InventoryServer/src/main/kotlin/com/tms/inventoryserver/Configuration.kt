package com.tms.inventoryserver

import com.tms.inventoryserver.entities.Item
import com.tms.inventoryserver.repositories.ItemRepository
import org.slf4j.Logger
import org.slf4j.LoggerFactory
import org.springframework.boot.CommandLineRunner
import org.springframework.context.annotation.Bean
import org.springframework.context.annotation.Configuration


@Configuration
internal class LoadDatabase {
    @Bean
    fun initDatabase(repository: ItemRepository): CommandLineRunner {
        return CommandLineRunner { args: Array<String?>? ->
            log.info("Preloading " + repository.save(Item("Syringe", false, "0000000000")))
            log.info("Preloading " + repository.save(Item("Gauze", false, "0000039321")))
            log.info("Preloading " + repository.save(Item("Gauze Dressing Pad", false, "0000030583")))
        }
    }

    companion object {
        private val log: Logger = LoggerFactory.getLogger(LoadDatabase::class.java)
    }
}