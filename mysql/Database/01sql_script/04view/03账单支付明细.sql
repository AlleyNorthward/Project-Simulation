-- @author 巷北  
-- @time 2026-01-01 21:58:33  

CREATE OR REPLACE VIEW v_BillPaymentDetail AS
SELECT 
    b.billId,
    b.checkInId,
    s.customerId,
    c.name AS customerName,
    b.roomCharge,
    b.diningCharge,
    b.otherCharge,
    b.totalAmount,
    IFNULL(SUM(p.paidAmount),0) AS paidAmount,
    b.totalAmount - IFNULL(SUM(p.paidAmount),0) AS unpaidAmount,
    GROUP_CONCAT(DISTINCT e.name) AS handledBy
FROM Bill b
JOIN Stay s ON b.checkInId = s.checkInId
JOIN Customer c ON s.customerId = c.customerId
LEFT JOIN Payment p ON p.billId = b.billId
LEFT JOIN Employee e ON p.operatorId = e.employeeId
GROUP BY b.billId, b.checkInId, s.customerId, c.name, b.roomCharge, b.diningCharge, b.otherCharge, b.totalAmount;
