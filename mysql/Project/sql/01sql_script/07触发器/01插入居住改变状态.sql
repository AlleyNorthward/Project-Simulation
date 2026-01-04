-- @author 巷北  
-- @time 2026-01-01 22:40:30  

DELIMITER $$
CREATE TRIGGER trg_stay_after_insert_set_room_and_reservation
AFTER INSERT ON Stay
FOR EACH ROW
BEGIN
  -- 将房间状态改为 已入住
  UPDATE Room
  SET status = '已入住'
  WHERE roomId = NEW.roomId;

  -- 若来源于预约，则把对应预约状态设置为 已入住
  IF NEW.reservationId IS NOT NULL THEN
    UPDATE Reservation
    SET status = '已入住'
    WHERE reservationId = NEW.reservationId;
  END IF;
END$$
DELIMITER ;












