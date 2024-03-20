import { Component } from '@angular/core';
import {DataService} from "../data.service";

@Component({
  selector: 'app-config',
  templateUrl: './config.component.html',
  styleUrl: './config.component.scss'
})
export class ConfigComponent {
  displayedColumns = ["name", "present", "active"]
  data: any;

  constructor(private dataService: DataService) {
  }

  changeActive(id: string, active: boolean) {
    this.dataService.setItemActive(id, active);
  }

  ngOnInit() {
    setInterval(() => {
      this.dataService.getAllItems().subscribe(response => {
        this.data = response;
      });
    }, 1000);
  }
}
