import { Component } from '@angular/core';

import {DataService} from "../data.service";
import {MatProgressSpinnerModule} from "@angular/material/progress-spinner";
@Component({
  selector: 'app-items',
  templateUrl: './items.component.html',
  styleUrl: './items.component.scss'
})
export class ItemsComponent {

  constructor(private dataService: DataService) {
  }

  displayedColumns = ['name', 'present'];
  data: any;
  numPresent = 0;

  ngOnInit() {
    setInterval(() => {
      this.dataService.getData().subscribe(response => {
        this.data = response;
        var counter = 0;
        for (var i = 0; i < response.length; i++) {
          if (response[i].isPresent) {
            counter++;
          }
        }
        this.numPresent = counter;
      });
    }, 1000);
  }


}
