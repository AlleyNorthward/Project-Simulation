CREATE OR REPLACE VIEW v_CurrentStay AS
SELECT 
    s.checkInId,
    c.customerId,
    c.name AS customerName,
    r.roomId,
    rt.name AS roomType,
    s.checkInTime,
    s.plannedCheckOutTime,
    s.status AS stayStatus,
    e.name AS handledBy
FROM Stay s
JOIN Customer c ON s.customerId = c.customerId
JOIN Room r ON s.roomId = r.roomId
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
LEFT JOIN Employee e ON s.staffId = e.employeeId
WHERE s.status = '在住';

CREATE OR REPLACE VIEW v_RoomAvailability AS
SELECT 
    rt.roomTypeId,
    rt.name AS roomTypeName,
    COUNT(r.roomId) AS totalRooms,
    SUM(CASE WHEN r.status = '空闲' THEN 1 ELSE 0 END) AS freeRooms,
    ROUND(SUM(CASE WHEN r.status = '空闲' THEN 1 ELSE 0 END)/COUNT(r.roomId)*100,2) AS freeRate
FROM Room r
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
GROUP BY rt.roomTypeId, rt.name;

CREATE OR REPLACE VIEW v_CustomerConsumption AS
SELECT
    c.customerId,
    c.name AS customerName,
    ml.levelName AS membershipLevel,
    IFNULL(SUM(cr.amount), 0) AS totalConsumption,
    COUNT(DISTINCT s.checkInId) AS stayCount
FROM Customer c
LEFT JOIN MembershipLevel ml ON c.membershipLevelId = ml.levelId
LEFT JOIN Stay s ON s.customerId = c.customerId
LEFT JOIN ConsumptionRecord cr ON cr.checkInId = s.checkInId
GROUP BY c.customerId, c.name, ml.levelName;

CREATE OR REPLACE VIEW v_BillPaymentDetail AS
SELECT
    b.billId,
    b.checkInId,
    s.customerId,
    c.name AS customerName,
    b.roomCharge,
    b.consumptionCharge,
    b.otherCharge,
    b.totalAmount,
    IFNULL(SUM(p.paidAmount), 0) AS paidAmount,
    b.totalAmount - IFNULL(SUM(p.paidAmount), 0) AS unpaidAmount,
    GROUP_CONCAT(DISTINCT e.name) AS handledBy
FROM Bill b
JOIN Stay s ON b.checkInId = s.checkInId
JOIN Customer c ON s.customerId = c.customerId
LEFT JOIN Payment p ON p.billId = b.billId
LEFT JOIN Employee e ON p.operatorId = e.employeeId
GROUP BY b.billId, b.checkInId, s.customerId, c.name, b.roomCharge, b.consumptionCharge, b.otherCharge, b.totalAmount;

CREATE OR REPLACE VIEW v_RoomMaintenance AS
SELECT 
    m.maintenanceId,
    r.roomId,
    rt.name AS roomType,
    e.employeeId AS technicianId,
    e.name AS technicianName,
    m.startTime,
    m.endTime,
    m.problemDescription,
    m.status AS maintenanceStatus
FROM Maintenance m
JOIN Room r ON m.roomId = r.roomId
JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
JOIN Employee e ON m.employeeId = e.employeeId;

CREATE OR REPLACE VIEW v_MonthlyRevenue AS
SELECT 
    DATE_FORMAT(p.paymentTime, '%Y-%m') AS month,
    SUM(p.paidAmount) AS totalRevenue
FROM Payment p
GROUP BY DATE_FORMAT(p.paymentTime, '%Y-%m')
ORDER BY month;






