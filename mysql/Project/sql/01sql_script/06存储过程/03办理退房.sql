-- @author 巷北  
-- @time 2026-01-01 22:30:52  

DELIMITER $$
CREATE PROCEDURE sp_checkout_and_generate_bill(
  IN p_checkInId INT,
  IN p_otherCharge DECIMAL(10,2),
  IN p_discountAmount DECIMAL(10,2),
  OUT p_billId INT
)
BEGIN
  DECLARE v_roomId INT;
  DECLARE v_checkInTime DATETIME;
  DECLARE v_basePrice DECIMAL(10,2);
  DECLARE v_nights INT;
  DECLARE v_consumption DECIMAL(12,2);
  DECLARE v_roomCharge DECIMAL(12,2);
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    ROLLBACK;
    SET p_billId = NULL;
  END;

  START TRANSACTION;
    -- 1) 锁定 stay 行并获取 roomId/checkInTime
    SELECT roomId, checkInTime INTO v_roomId, v_checkInTime
    FROM Stay WHERE checkInId = p_checkInId FOR UPDATE;

    -- 2) 获取房型基础价
    SELECT rt.basePrice INTO v_basePrice
    FROM Room r JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
    WHERE r.roomId = v_roomId;

    -- 3) 计算 nights
    SET v_nights = TIMESTAMPDIFF(DAY, DATE(v_checkInTime), DATE(NOW()));
    IF v_nights <= 0 THEN
      SET v_nights = 1;
    END IF;

    SET v_roomCharge = v_basePrice * v_nights;

    -- 4) 汇总消费记录
    SELECT IFNULL(SUM(amount), 0) INTO v_consumption FROM ConsumptionRecord WHERE checkInId = p_checkInId;

    -- 5) 生成账单
    INSERT INTO Bill (checkInId, roomCharge, consumptionCharge, otherCharge, discountAmount, totalAmount, paymentStatus, createdTime)
    VALUES (p_checkInId, v_roomCharge, v_consumption, IFNULL(p_otherCharge,0.00), IFNULL(p_discountAmount,0.00),
            v_roomCharge + v_consumption + IFNULL(p_otherCharge,0.00) - IFNULL(p_discountAmount,0.00),
            '未支付', NOW());

    SET p_billId = LAST_INSERT_ID();

    -- 6) 更新 stay: 实际退房时间 & 状态
    UPDATE Stay SET actualCheckOutTime = NOW(), status = '已退房' WHERE checkInId = p_checkInId;

    -- 7) 释放房间为 空闲
    UPDATE Room SET status = '空闲' WHERE roomId = v_roomId;

  COMMIT;
END$$
DELIMITER ;

-- CALL sp_checkout_and_generate_bill(3, 0.00, 0.00, @billid);
-- SELECT @billid;












