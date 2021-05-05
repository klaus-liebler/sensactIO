import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SinglePWMCommandWidgetComponent } from './single-pwmcommand-widget.component';

describe('SinglePWMCommandWidgetComponent', () => {
  let component: SinglePWMCommandWidgetComponent;
  let fixture: ComponentFixture<SinglePWMCommandWidgetComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SinglePWMCommandWidgetComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SinglePWMCommandWidgetComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
