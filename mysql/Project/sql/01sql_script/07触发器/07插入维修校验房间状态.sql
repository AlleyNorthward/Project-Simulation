-- @author 巷北  
-- @time 2026-01-01 22:44:21  

DELIMITER $$
CREATE TRIGGER trg_maintenance_before_insert_check_room_status
BEFORE INSERT ON Maintenance
FOR EACH ROW
BEGIN
  DECLARE v_room_status ENUM('空闲','已预订','已入住','维修中');
  SELECT status INTO v_room_status FROM Room WHERE roomId = NEW.roomId;
  IF v_room_status = '已入住' THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Cannot create maintenance for an occupied room';
  END IF;
END$$
DELIMITER ;
