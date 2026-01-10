-- @author 巷北  
-- @time 2026-01-01 22:30:00  

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

    -- 检查房间是否空闲
    IF (SELECT status FROM Room WHERE roomId = p_roomId FOR UPDATE) <> '空闲' THEN
      ROLLBACK;
      SET p_checkInId = NULL;
      LEAVE proc_block;
    END IF;

    -- 插入入住记录
    INSERT INTO Stay (reservationId, customerId, roomId, checkInTime, plannedCheckOutTime, guestCount, depositAmount, status, staffId)
    VALUES (NULL, p_customerId, p_roomId, NOW(), DATE_ADD(NOW(), INTERVAL 1 DAY), p_guestCount, p_deposit, '在住', p_staffId);

    SET p_checkInId = LAST_INSERT_ID();

    -- 更新房间状态
    UPDATE Room SET status = '已入住' WHERE roomId = p_roomId;

  COMMIT;

END$$

DELIMITER ;

-- CALL sp_direct_check_in(3, 101, 1, 50.00, 1, @cid);
-- SELECT @cid;
