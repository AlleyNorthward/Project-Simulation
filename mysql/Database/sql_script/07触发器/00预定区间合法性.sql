-- @author 巷北  
-- @time 2026-01-01 22:39:37  

DELIMITER $$
CREATE TRIGGER trg_reservation_before_insert_check_dates
BEFORE INSERT ON Reservation
FOR EACH ROW
BEGIN
  IF NEW.checkOutDate <= NEW.checkInDate THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Reservation checkOutDate must be > checkInDate';
  END IF;
END$$
DELIMITER ;
