-- @author 巷北  
-- @time 2026-01-01 22:22:46  

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


-- 调用方法. 
-- CALL sp_make_reservation(1, 2, '2025-06-10', '2025-06-12', 1, 200.00, @resid);
-- SELECT @resid;















