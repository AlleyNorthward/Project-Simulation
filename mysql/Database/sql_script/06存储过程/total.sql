DELIMITER $$
CREATE PROCEDURE sp_make_reservation(
  IN p_customerId INT,
  IN p_roomTypeId INT,
  IN p_checkInDate DATE,
  IN p_checkOutDate DATE,
  IN p_reservedRoomCount INT,
  IN p_depositAmount DECIMAL(10,2),
  OUT p_reservationId INT
)
BEGIN
  DECLARE EXIT HANDLER FOR SQLEXCEPTION 
  BEGIN
    ROLLBACK;
    SET p_reservationId = NULL;
  END;

  START TRANSACTION;
    INSERT INTO Reservation
      (customerId, roomTypeId, reservedRoomCount, checkInDate, checkOutDate, status, depositAmount)
    VALUES
      (p_customerId, p_roomTypeId, p_reservedRoomCount, p_checkInDate, p_checkOutDate, '已预订', p_depositAmount);

    SET p_reservationId = LAST_INSERT_ID();
  COMMIT;
END$$
DELIMITER ;

DELIMITER $$

CREATE PROCEDURE sp_direct_check_in(
  IN p_customerId INT,
  IN p_roomId INT,
  IN p_guestCount INT,
  IN p_deposit DECIMAL(10,2),
  IN p_staffId INT,
  OUT p_checkInId INT
)
proc_block: BEGIN

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    ROLLBACK;
    SET p_checkInId = NULL;
  END;

  START TRANSACTION;

    IF (SELECT status FROM Room WHERE roomId = p_roomId FOR UPDATE) <> '空闲' THEN
      ROLLBACK;
      SET p_checkInId = NULL;
      LEAVE proc_block; 
    END IF;

    INSERT INTO Stay (reservationId, customerId, roomId, checkInTime, plannedCheckOutTime, guestCount, depositAmount, status, staffId)
    VALUES (NULL, p_customerId, p_roomId, NOW(), DATE_ADD(NOW(), INTERVAL 1 DAY), p_guestCount, p_deposit, '在住', p_staffId);

    SET p_checkInId = LAST_INSERT_ID();

    UPDATE Room SET status = '已入住' WHERE roomId = p_roomId;

  COMMIT;

END$$

DELIMITER ;

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
    SELECT roomId, checkInTime INTO v_roomId, v_checkInTime
    FROM Stay WHERE checkInId = p_checkInId FOR UPDATE;

    SELECT rt.basePrice INTO v_basePrice
    FROM Room r JOIN RoomType rt ON r.roomTypeId = rt.roomTypeId
    WHERE r.roomId = v_roomId;

    SET v_nights = TIMESTAMPDIFF(DAY, DATE(v_checkInTime), DATE(NOW()));
    IF v_nights <= 0 THEN
      SET v_nights = 1;
    END IF;

    SET v_roomCharge = v_basePrice * v_nights;

    SELECT IFNULL(SUM(amount), 0) INTO v_consumption FROM ConsumptionRecord WHERE checkInId = p_checkInId;

    INSERT INTO Bill (checkInId, roomCharge, consumptionCharge, otherCharge, discountAmount, totalAmount, paymentStatus, createdTime)
    VALUES (p_checkInId, v_roomCharge, v_consumption, IFNULL(p_otherCharge,0.00), IFNULL(p_discountAmount,0.00),
            v_roomCharge + v_consumption + IFNULL(p_otherCharge,0.00) - IFNULL(p_discountAmount,0.00),
            '未支付', NOW());

    SET p_billId = LAST_INSERT_ID();

    UPDATE Stay SET actualCheckOutTime = NOW(), status = '已退房' WHERE checkInId = p_checkInId;

    UPDATE Room SET status = '空闲' WHERE roomId = v_roomId;

  COMMIT;
END$$
DELIMITER ;


DELIMITER $$
CREATE PROCEDURE sp_add_payment(
  IN p_billId INT,
  IN p_paidAmount DECIMAL(10,2),
  IN p_method VARCHAR(20),
  IN p_operatorId INT,
  OUT p_paymentId INT
)
BEGIN
  DECLARE v_totalBill DECIMAL(12,2);
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    ROLLBACK;
    SET p_paymentId = NULL;
  END;

  START TRANSACTION;
    SELECT totalAmount INTO v_totalBill FROM Bill WHERE billId = p_billId FOR UPDATE;

    INSERT INTO Payment (billId, paidAmount, paymentMethod, paymentTime, operatorId, remarks)
    VALUES (p_billId, p_paidAmount, p_method, NOW(), p_operatorId, NULL);

    SET p_paymentId = LAST_INSERT_ID();

    SELECT IFNULL(SUM(paidAmount), 0) INTO v_sumPaid FROM Payment WHERE billId = p_billId;

    IF v_sumPaid >= v_totalBill THEN
      UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = p_billId;
    ELSEIF v_sumPaid > 0 THEN
      UPDATE Bill SET paymentStatus = '部分支付' WHERE billId = p_billId;
    ELSE
      UPDATE Bill SET paymentStatus = '未支付' WHERE billId = p_billId;
    END IF;

  COMMIT;
END$$
DELIMITER ;


DELIMITER $$
CREATE PROCEDURE sp_promote_membership_based_on_spending(
  IN p_customerId INT,
  OUT p_newLevelId INT
)
BEGIN
  DECLARE v_totalPaid DECIMAL(12,2);

  SELECT IFNULL(SUM(p.paidAmount),0) INTO v_totalPaid
  FROM Payment p
  JOIN Bill b ON p.billId = b.billId
  JOIN Stay s ON b.checkInId = s.checkInId
  WHERE s.customerId = p_customerId;

  SELECT levelId INTO p_newLevelId
  FROM MembershipLevel
  WHERE requiredSpending <= v_totalPaid
  ORDER BY requiredSpending DESC
  LIMIT 1;

  IF p_newLevelId IS NOT NULL THEN
    UPDATE Customer SET membershipLevelId = p_newLevelId WHERE customerId = p_customerId;
  END IF;
END$$
DELIMITER ;
