import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { MatPaginatorModule } from "@angular/material/paginator";
import { MatProgressSpinnerModule } from "@angular/material/progress-spinner";
import { MatSortModule } from "@angular/material/sort";
import { MatTableModule} from "@angular/material/table";
import { MatToolbarModule } from "@angular/material/toolbar";

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import {HttpClientModule} from "@angular/common/http";
import { provideAnimationsAsync } from '@angular/platform-browser/animations/async';
import { ConfigComponent } from './config/config.component';
import { ItemsComponent } from './items/items.component';
import {MatCheckbox} from "@angular/material/checkbox";

@NgModule({
  declarations: [
    AppComponent,
    ConfigComponent,
    ItemsComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    MatTableModule,
    MatPaginatorModule,
    MatSortModule,
    MatProgressSpinnerModule,
    MatToolbarModule,
    MatCheckbox
  ],
  providers: [
    provideAnimationsAsync()
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
