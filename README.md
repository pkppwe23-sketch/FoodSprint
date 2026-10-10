# FoodSprint — Food Delivery LLD

A small **C++17 Low-Level Design (LLD)** project for a food-delivery app. It demonstrates how a customer can find the nearest open restaurant and the nearest available delivery partner, then place an order.

## Features
- Find the nearest **open** restaurant using the customer's location.
- Find the nearest **available** delivery partner using the restaurant's location.
- Place a basic order and assign a delivery partner.
- Keep the design beginner-friendly with small classes and one simple Strategy pattern.
- Uses sample in-memory data; no database, maps API, payment gateway, or real GPS integration is required.

## Design
- `Location`: latitude and longitude.
- `Restaurant`: restaurant details, location, open/closed status.
- `DeliveryPartner`: partner details, location, availability.
- `Order`: customer, restaurant, partner, and order status.
- `NearestEntityStrategy`: Strategy interface for choosing the closest candidate.
- `NearestAvailableStrategy`: finds the closest candidate that is available.
- `FoodSprintService`: coordinates restaurant search, partner assignment, and order placement.

Distance is calculated with the Haversine formula. The sample uses approximate coordinates and is for learning, not production navigation.

## Build and run

Requires a C++17 compiler.

### Windows (MinGW g++)
```bash
g++ -std=c++17 src/main.cpp -o foodsprint
./foodsprint
```

In Windows PowerShell, run `./foodsprint.exe` after compiling.

### Linux / macOS
```bash
g++ -std=c++17 src/main.cpp -o foodsprint
./foodsprint
```

## Example flow
1. Customer selects food.
2. FoodSprint finds the nearest open restaurant.
3. FoodSprint finds the nearest available delivery partner near that restaurant.
4. An order is created and the partner is marked unavailable.

## Scope and next improvements
This is an LLD demonstration with hard-coded sample data. A real application would need persistent storage, authentication, order/payment validation, concurrency-safe partner assignment, accurate maps/routing, and API endpoints.
