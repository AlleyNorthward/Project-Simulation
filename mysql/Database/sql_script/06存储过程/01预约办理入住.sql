-- @author 巷北  
-- @time 2026-01-01 22:26:54  
DELIMITER $$

CREATE PROCEDURE sp_check_in_from_reservation(
    IN p_reservationId INT,
    IN p_roomId INT,
    IN p_staffId INT,
    OUT p_checkInId INT
)
BEGIN
    DECLARE v_roomTypeId INT;
    DECLARE v_res_roomTypeId INT;
    DECLARE v_customerId INT;
    DECLARE v_deposit DECIMAL(10,2);
    DECLARE v_planned DATE;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        SET p_checkInId = NULL;
    END;

    START TRANSACTION;

    SELECT roomTypeId INTO v_roomTypeId
    FROM Room
    WHERE roomId = p_roomId
    FOR UPDATE;

    SELECT customerId, roomTypeId, depositAmount, checkOutDate
    INTO v_customerId, v_res_roomTypeId, v_deposit, v_planned
    FROM Reservation
    WHERE reservationId = p_reservationId
    FOR UPDATE;

    IF v_roomTypeId IS NULL OR v_res_roomTypeId IS NULL OR v_roomTypeId <> v_res_roomTypeId THEN
        ROLLBACK;
        SET p_checkInId = NULL;
        COMMIT;
        LEAVE;
    END IF;

    INSERT INTO Stay (reservationId, customerId, roomId, checkInTime, plannedCheckOutTime, guestCount, depositAmount, status, staffId)
    VALUES (p_reservationId, v_customerId, p_roomId, NOW(), CONCAT(v_planned,' 12:00:00'), 1, v_deposit, '在住', p_staffId);

    SET p_checkInId = LAST_INSERT_ID();

    UPDATE Room SET status = '已入住' WHERE roomId = p_roomId;

    COMMIT;

END$$

DELIMITER ;
-- CALL sp_check_in_from_reservation(1, 101, 3, @newCheckInId);
-- SELECT @newCheckInId;
