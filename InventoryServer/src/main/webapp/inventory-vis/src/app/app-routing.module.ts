import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {ConfigComponent} from "./config/config.component";
import {AppComponent} from "./app.component";
import {ItemsComponent} from "./items/items.component";

const routes: Routes = [
  {path: "", component: ItemsComponent},
  {path: "config", component: ConfigComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
