import { Injectable } from '@angular/core';
import {HttpClient, HttpErrorResponse} from "@angular/common/http";
import {catchError, Observable, throwError} from "rxjs";

export class Item {
  constructor(
    public name: string,
    public isPresent: boolean,
    public id: string,
    public scannerId: string,
    public expiresAt: Date,
    public active: boolean
) {

  }
}

@Injectable({
  providedIn: 'root'
})
export class DataService {

  constructor(private http: HttpClient) { }


  getData(): Observable<Item[]> {
    return this.http.get<Item[]>('/api/items/active');
  }

  getAllItems(): Observable<Item[]> {
    return this.http.get<Item[]>('/api/items');
  }

  private handleError(error: HttpErrorResponse) {
    if (error.status === 0) {
      // A client-side or network error occurred. Handle it accordingly.
      console.error('An error occurred:', error.error);
    } else {
      // The backend returned an unsuccessful response code.
      // The response body may contain clues as to what went wrong.
      console.error(
        `Backend returned code ${error.status}, body was: `, error.error);
    }
    // Return an observable with a user-facing error message.
    return throwError(() => new Error('Something bad happened; please try again later.'));
  }

  setItemActive(id: string, active: boolean) {
    console.log(`/api/items/enable/${id}`)
    this.http.post<boolean>(`/api/items/enable/${id}`, active).pipe(
      catchError(this.handleError)
    ).subscribe();
  }
}
