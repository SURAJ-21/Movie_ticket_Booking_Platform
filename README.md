# Real-Time Movie Ticket Booking Platform (OS mini-Project)

## Introduction
This project is a **Real-Time Movie Ticket Booking Platform** designed to handle high concurrency, advanced inter-process communication (IPC), multithreading, and semaphores. The system offers a robust and feature-rich platform for real-time movie ticket booking, ensuring fault tolerance, optimized performance, and dynamic pricing.

## Key Features
1. **Concurrency Control**: Ensures proper handling of multiple users trying to book the same seat. The system uses a First Come First Serve (FCFS) mechanism for conflict resolution.
2. **Inter-Process Communication (IPC)**: Efficient communication between different components using IPC.
3. **Multithreading & Multiprocessing**: Optimizes the system's responsiveness and scalability by parallel processing.
4. **Dynamic Pricing**: Ticket prices are dynamically adjusted based on festivals, seasons, and demand.
5. **Interactive Payment System**: Secure and seamless transaction processing for a smooth user experience.
6. **Robust User Authentication**: Ensures a secure environment for both users and administrators.
7. **Scaling**: Designed to handle increased load with efficient scaling strategies.
8. **Fault Tolerance**: Implemented using redundant servers to ensure continuous operation in the event of failures.
9. **Optimized System Performance**: Uses a multi-level indexing approach to enhance performance.
10. **Admin Component**: Plays a dual role by acting as both a server for lower client processes and a client to the main server.

## System Components
### 1. Main Server
- Handles the core operations of the platform, managing movie data, seating availability, pricing, and client requests.
- Facilitates IPC and handles concurrent client connections.

### 2. Admin Component
- Acts as a server for client components (users) and concurrently functions as a client for the main server.
- Manages user bookings, dynamically adjusts pricing, and enforces concurrency control.
- Responsible for maintaining fault tolerance and interacting with redundant servers.

### 3. Client Component
- The user-facing interface where customers can view movies, book tickets, and complete payments.
- Communicates with the admin component to process bookings and display dynamic pricing.

## Concurrency Control
When multiple users attempt to book the same seat, the platform employs an FCFS (First Come First Serve) policy to ensure fairness. Semaphores are used to maintain the integrity of seat allocation.

## Dynamic Pricing
Ticket prices change dynamically based on real-time factors such as:
- Festivals
- Seasons
- User demand

## Fault-Tolerance
The system uses redundant servers to ensure high availability and fault tolerance. If one server goes down, the redundant server will automatically take over without interrupting the user experience.

## Optimized Performance
The system uses a **multi-level indexing** strategy to speed up database queries and ensure quick retrieval of movie and seating information.

## Technologies Used
- **Concurrency & IPC**: Implemented using semaphores and multithreading.
- **Server Components**: Designed with a dual-role admin component that serves both server and client functionalities.
- **Dynamic Pricing**: Algorithm-based price adjustments.
- **Scaling & Fault-Tolerance**: Managed using redundant servers and optimized load balancing.
  
## Usage
1. **Run the Main Server**: Start the main server which manages all core functionalities.
2. **Admin Server**: Run the admin component that interacts with both clients and the main server.
3. **Client Access**: Users can log in, view movies, and book tickets in real-time.
4. **Fault-Tolerance**: Monitor system redundancy to ensure smooth operation.

## Installation
1. Clone the repository.
2. Set up the necessary environment (IPC, semaphores, etc.).
3. Configure the servers (main and redundant).
4. Run the main server, admin component, and client processes.

## Future Enhancements
- **Machine Learning for Predictive Pricing**: Use ML models to predict demand and adjust pricing more accurately.
- **Geo-Location Based Dynamic Pricing**: Adjust pricing based on user location.
- **User Reviews and Feedback**: Allow users to rate movies and share feedback.

---

This system ensures real-time booking with advanced features like dynamic pricing, fault tolerance, and robust concurrency control, making it ideal for high-demand situations like festivals or new movie releases.
