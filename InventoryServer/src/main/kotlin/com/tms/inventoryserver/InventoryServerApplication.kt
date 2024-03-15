package com.tms.inventoryserver

import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication
import org.springframework.context.annotation.ComponentScan

@SpringBootApplication
@ComponentScan
class InventoryServerApplication

fun main(args: Array<String>) {
    runApplication<InventoryServerApplication>(*args)
}
