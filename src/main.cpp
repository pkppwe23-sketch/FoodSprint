#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Location {
    double latitude;
    double longitude;
};

double distanceInKm(const Location& a, const Location& b) {
    const double earthRadiusKm = 6371.0;
    const double pi = acos(-1.0);

    double lat1 = a.latitude * pi / 180.0;
    double lat2 = b.latitude * pi / 180.0;
    double deltaLat = (b.latitude - a.latitude) * pi / 180.0;
    double deltaLon = (b.longitude - a.longitude) * pi / 180.0;

    double value = sin(deltaLat / 2) * sin(deltaLat / 2)
        + cos(lat1) * cos(lat2)
        * sin(deltaLon / 2) * sin(deltaLon / 2);

    double angle = 2 * atan2(sqrt(value), sqrt(1 - value));
    return earthRadiusKm * angle;
}

struct Restaurant {
    int id;
    string name;
    Location location;
    bool isOpen;
};

struct DeliveryPartner {
    int id;
    string name;
    Location location;
    bool isAvailable;
};

struct Customer {
    int id;
    string name;
    Location location;
};

enum class OrderStatus {
    PLACED,
    PREPARING,
    PICKED_UP,
    DELIVERED
};

string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PLACED: return "Placed";
        case OrderStatus::PREPARING: return "Preparing";
        case OrderStatus::PICKED_UP: return "Picked up";
        case OrderStatus::DELIVERED: return "Delivered";
    }
    return "Unknown";
}

struct Order {
    int id;
    Customer customer;
    Restaurant restaurant;
    DeliveryPartner partner;
    string foodItem;
    OrderStatus status;
};

// Strategy Pattern: defines how the closest eligible item is selected.
template <typename T>
class NearestEntityStrategy {
public:
    virtual ~NearestEntityStrategy() = default;
    virtual const T* findNearest(
        const Location& point,
        const vector<T>& candidates
    ) const = 0;
};

template <typename T>
class NearestAvailableStrategy : public NearestEntityStrategy<T> {
public:
    const T* findNearest(
        const Location& point,
        const vector<T>& candidates
    ) const override {
        const T* nearest = nullptr;
        double bestDistance = numeric_limits<double>::max();

        for (const T& candidate : candidates) {
            if (!isEligible(candidate)) {
                continue;
            }

            double distance = distanceInKm(point, candidate.location);
            if (distance < bestDistance) {
                bestDistance = distance;
                nearest = &candidate;
            }
        }

        return nearest;
    }

private:
    bool isEligible(const Restaurant& restaurant) const {
        return restaurant.isOpen;
    }

    bool isEligible(const DeliveryPartner& partner) const {
        return partner.isAvailable;
    }
};

class FoodSprintService {
public:
    FoodSprintService(
        vector<Restaurant> restaurants,
        vector<DeliveryPartner> partners
    ) : restaurants(move(restaurants)), partners(move(partners)) {}

    const Restaurant* findNearestRestaurant(const Location& customerLocation) const {
        return restaurantStrategy.findNearest(customerLocation, restaurants);
    }

    const DeliveryPartner* findNearestPartner(const Location& restaurantLocation) const {
        return partnerStrategy.findNearest(restaurantLocation, partners);
    }

    bool placeOrder(
        int orderId,
        const Customer& customer,
        const string& foodItem
    ) {
        const Restaurant* restaurant = findNearestRestaurant(customer.location);

        if (restaurant == nullptr) {
            cout << "Sorry, no open restaurant is currently available.\n";
            return false;
        }

        const DeliveryPartner* partner = findNearestPartner(restaurant->location);

        if (partner == nullptr) {
            cout << "Sorry, no delivery partner is currently available.\n";
            return false;
        }

        // Update the stored partner so they cannot be assigned twice in this demo.
        for (DeliveryPartner& storedPartner : partners) {
            if (storedPartner.id == partner->id) {
                storedPartner.isAvailable = false;
                break;
            }
        }

        Order order{
            orderId,
            customer,
            *restaurant,
            *partner,
            foodItem,
            OrderStatus::PLACED
        };

        printOrder(order);
        return true;
    }

private:
    vector<Restaurant> restaurants;
    vector<DeliveryPartner> partners;

    NearestAvailableStrategy<Restaurant> restaurantStrategy;
    NearestAvailableStrategy<DeliveryPartner> partnerStrategy;

    void printOrder(const Order& order) const {
        cout << "\n========== FoodSprint Order ==========" << '\n';
        cout << "Order ID: " << order.id << '\n';
        cout << "Customer: " << order.customer.name << '\n';
        cout << "Food: " << order.foodItem << '\n';
        cout << "Nearest open restaurant: " << order.restaurant.name << '\n';
        cout << "Distance to restaurant: "
             << fixed << setprecision(2)
             << distanceInKm(order.customer.location, order.restaurant.location)
             << " km\n";
        cout << "Assigned delivery partner: " << order.partner.name << '\n';
        cout << "Partner distance from restaurant: "
             << fixed << setprecision(2)
             << distanceInKm(order.restaurant.location, order.partner.location)
             << " km\n";
        cout << "Order status: " << statusToString(order.status) << '\n';
        cout << "======================================\n";
    }
};

int main() {
    Customer customer{
        1,
        "Aarav",
        {22.2500, 84.9000}
    };

    vector<Restaurant> restaurants{
        {1, "Spice Corner", {22.2520, 84.9020}, true},
        {2, "Burger Station", {22.2600, 84.9100}, true},
        {3, "Closed Cafe", {22.2510, 84.9010}, false}
    };

    vector<DeliveryPartner> partners{
        {1, "Ravi", {22.2530, 84.9030}, true},
        {2, "Priya", {22.2580, 84.9080}, true},
        {3, "Imran", {22.2490, 84.8970}, false}
    };

    FoodSprintService foodSprint(restaurants, partners);
    foodSprint.placeOrder(1001, customer, "Veg Burger");

    return 0;
}
