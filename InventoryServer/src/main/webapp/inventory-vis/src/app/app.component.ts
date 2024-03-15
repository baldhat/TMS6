import { Component } from '@angular/core';
import {DataService} from "./data.service";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrl: './app.component.scss'
})
export class AppComponent {
  title = 'inventory-vis';
  displayedColumns = ['name', 'id', 'present'];
  data: any;

  constructor(private dataService: DataService) {}

  ngOnInit() {
    setInterval(() => {
      this.dataService.getData().subscribe(response => {
        this.data = response;
      });
    }, 1000);


  }
}
