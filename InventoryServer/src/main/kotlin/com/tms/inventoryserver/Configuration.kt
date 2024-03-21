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
            log.info("Preloading " + repository.save(Item("First Aid Dressing 1", false, "0000000006", "1")))
            //log.info("Preloading " + repository.save(Item("Instant Cold Pack", false, "0000000007", "1")))
            log.info("Preloading " + repository.save(Item("Triangular Bandage 1", false, "0000000022", "1")))
            log.info("Preloading " + repository.save(Item("Elastic Bandage 1", false, "0000000011", "1")))
            log.info("Preloading " + repository.save(Item("First Aid Dressing 2", false, "0000000009", "1")))
            log.info("Preloading " + repository.save(Item("Wound Plasters 1", false, "0000000005", "1")))
            log.info("Preloading " + repository.save(Item("Nonwoven Swabs 1", false, "0000000002", "1")))
            log.info("Preloading " + repository.save(Item("Nonwoven Swabs 2", false, "0000000020", "1")))
            log.info("Preloading " + repository.save(Item("First Aid Dressing 3", false, "0000000015", "1")))
            log.info("Preloading " + repository.save(Item("First Aid Dressing 4", false, "0000000019", "1")))
            //log.info("Preloading " + repository.save(Item("Medical Tape", false, "0000000013", "1")))
            //log.info("Preloading " + repository.save(Item("Medical Scissors", false, "0000000001", "1")))
            log.info("Preloading " + repository.save(Item("Wound Plasters 2", false, "0000000008", "1")))
            log.info("Preloading " + repository.save(Item("Triangular Bandage 2", false, "0000000018", "1")))
            log.info("Preloading " + repository.save(Item("Disposable Vinyl Gloves", false, "0000000021", "1")))
            log.info("Preloading " + repository.save(Item("First Aid Dressing 5", false, "0000000014", "1")))
            log.info("Preloading " + repository.save(Item("Elastic BAndage 2", false, "0000000004", "1")))
            log.info("Preloading " + repository.save(Item("Hygienic Mouth Drape", false, "0000000016", "1")))
            log.info("Preloading " + repository.save(Item("Foliage Bags", false, "0000000012", "1")))
            log.info("Preloading " + repository.save(Item("Defibrillator", false, "0000000017", "1")))
        }
    }

    companion object {
        private val log: Logger = LoggerFactory.getLogger(LoadDatabase::class.java)
    }
}