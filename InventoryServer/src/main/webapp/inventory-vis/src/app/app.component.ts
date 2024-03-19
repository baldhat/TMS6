import { Component } from '@angular/core';
import {DataService} from "./data.service";
import {MatProgressSpinnerModule} from "@angular/material/progress-spinner";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent {
  title = 'inventory-vis';
  displayedColumns = ['name', 'present'];
  data: any;
  numPresent = 0;

  constructor(private dataService: DataService) {}

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
        console.log(this.numPresent)
      });
    }, 1000);


  }
}
