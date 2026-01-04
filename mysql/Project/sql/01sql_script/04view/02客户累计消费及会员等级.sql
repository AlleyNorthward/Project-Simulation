-- @author 巷北  
-- @time 2026-01-01 21:50:57  


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
