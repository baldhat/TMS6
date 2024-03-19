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
            log.info("Preloading " + repository.save(Item("Triangular Bandage", false, "0000000001", "1")))
            log.info("Preloading " + repository.save(Item("Adhesive Tape", false, "0000000002", "1")))
            log.info("Preloading " + repository.save(Item("Band Aids", false, "0000000003", "1")))
            log.info("Preloading " + repository.save(Item("Bandages", false, "0000000004", "1")))
            log.info("Preloading " + repository.save(Item("Emergency Blanket", false, "0000000005", "1")))
            log.info("Preloading " + repository.save(Item("Tweezers", false, "0000000006", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "0000000007", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "0000000008", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "0000000009", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "0000000010", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "3333333333", "1")))
            log.info("Preloading " + repository.save(Item("Gauze Pads", false, "0777777777", "1")))

        }
    }

    companion object {
        private val log: Logger = LoggerFactory.getLogger(LoadDatabase::class.java)
    }
}