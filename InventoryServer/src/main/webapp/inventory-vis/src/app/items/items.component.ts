import { Component } from '@angular/core';

import {DataService, Item} from "../data.service";
import {MatProgressSpinnerModule} from "@angular/material/progress-spinner";
@Component({
  selector: 'app-items',
  templateUrl: './items.component.html',
  styleUrl: './items.component.scss'
})
export class ItemsComponent {

  constructor(private dataService: DataService) {
  }

  displayedColumns = ['name', 'expiresAt', 'present'];
  data: Item[] = [];
  numPresent = 0;
  today = Date.now()

  ngOnInit() {
    setInterval(() => {
      this.dataService.getData().subscribe(response => {
        this.data = response;
        var counter = 0;
        for (var i = 0; i < response.length; i++) {
          this.data[i].expiresAt = new Date(response[i].expiresAt)
          if (response[i].isPresent) {
            counter++;
          }
        }
        this.numPresent = counter;
      });
    }, 1000);
  }


  protected readonly Date = Date;
}
