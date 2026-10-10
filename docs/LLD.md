# FoodSprint LLD

## Requirements
1. Customer can search for a nearby restaurant.
2. Only open restaurants are eligible.
3. The system selects the nearest available delivery partner near the selected restaurant.
4. The system creates an order and marks the selected partner unavailable.

## Main classes
- **Location**: stores latitude and longitude.
- **Customer**: customer ID, name, and current location.
- **Restaurant**: ID, name, location, and open status.
- **DeliveryPartner**: ID, name, location, and availability status.
- **Order**: order ID, customer, restaurant, partner, food item, and status.
- **FoodSprintService**: coordinates the use case.
- **NearestEntityStrategy**: strategy interface for nearest-candidate selection.
- **NearestAvailableStrategy**: implements nearest eligible candidate selection.

## Order flow
```text
Customer
   |
   v
FoodSprintService
   |
   v
Find nearest OPEN restaurant
   |
   v
Find nearest AVAILABLE partner near restaurant
   |
   v
Create order and mark partner unavailable
   |
   v
Show order summary
```

## Design pattern
**Strategy Pattern**: the nearest-candidate algorithm is represented by a strategy. This keeps the selection behavior separate from the service that places orders. The current strategy checks eligibility and calculates geographic distance.

## Simplifications
- Data is stored in vectors and disappears when the program exits.
- Distances use straight-line geographic distance, not actual road distance.
- Partner assignment is not concurrency-safe; a production system must reserve a partner atomically.
- Payment, authentication, cancellation, restaurant menus, and live GPS tracking are intentionally out of scope.
