-- @author 巷北  
-- @time 2026-01-01 22:41:57  


DELIMITER $$
CREATE TRIGGER trg_stay_after_update_release_room
AFTER UPDATE ON Stay
FOR EACH ROW
BEGIN
  IF NEW.status = '已退房' AND OLD.status <> '已退房' THEN
    UPDATE Room
    SET status = '空闲'
    WHERE roomId = NEW.roomId;
  END IF;
END$$
DELIMITER ;
