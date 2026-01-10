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

DELIMITER $$
CREATE TRIGGER trg_stay_after_insert_set_room_and_reservation
AFTER INSERT ON Stay
FOR EACH ROW
BEGIN
  UPDATE Room
  SET status = '已入住'
  WHERE roomId = NEW.roomId;

  IF NEW.reservationId IS NOT NULL THEN
    UPDATE Reservation
    SET status = '已入住'
    WHERE reservationId = NEW.reservationId;
  END IF;
END$$
DELIMITER ;

DELIMITER $$
CREATE TRIGGER trg_stay_before_update_set_actual_checkout
BEFORE UPDATE ON Stay
FOR EACH ROW
BEGIN
  IF NEW.status = '已退房' AND OLD.status <> '已退房' THEN
    IF NEW.actualCheckOutTime IS NULL THEN
      SET NEW.actualCheckOutTime = NOW();
    END IF;
  END IF;
END$$
DELIMITER ;

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

DELIMITER $$
CREATE TRIGGER trg_payment_after_insert_update_bill
AFTER INSERT ON Payment
FOR EACH ROW
BEGIN
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE v_total DECIMAL(12,2);

  SELECT IFNULL(SUM(paidAmount),0) INTO v_sumPaid FROM Payment WHERE billId = NEW.billId;
  SELECT totalAmount INTO v_total FROM Bill WHERE billId = NEW.billId;

  IF v_sumPaid >= v_total THEN
    UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = NEW.billId;
  ELSEIF v_sumPaid > 0 THEN
    UPDATE Bill SET paymentStatus = '部分支付', settledTime = NULL WHERE billId = NEW.billId;
  ELSE
    UPDATE Bill SET paymentStatus = '未支付', settledTime = NULL WHERE billId = NEW.billId;
  END IF;
END$$
DELIMITER ;

DELIMITER $$
CREATE TRIGGER trg_payment_after_update_recalc_bill
AFTER UPDATE ON Payment
FOR EACH ROW
BEGIN
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE v_total DECIMAL(12,2);
  DECLARE v_billId INT;

  SET v_billId = NEW.billId;

  SELECT IFNULL(SUM(paidAmount),0) INTO v_sumPaid FROM Payment WHERE billId = v_billId;
  SELECT totalAmount INTO v_total FROM Bill WHERE billId = v_billId;

  IF v_sumPaid >= v_total THEN
    UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = v_billId;
  ELSEIF v_sumPaid > 0 THEN
    UPDATE Bill SET paymentStatus = '部分支付', settledTime = NULL WHERE billId = v_billId;
  ELSE
    UPDATE Bill SET paymentStatus = '未支付', settledTime = NULL WHERE billId = v_billId;
  END IF;
END$$
DELIMITER ;

DELIMITER $$
CREATE TRIGGER trg_payment_after_delete_recalc_bill
AFTER DELETE ON Payment
FOR EACH ROW
BEGIN
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE v_total DECIMAL(12,2);
  DECLARE v_billId INT;

  SET v_billId = OLD.billId;

  SELECT IFNULL(SUM(paidAmount),0) INTO v_sumPaid FROM Payment WHERE billId = v_billId;
  SELECT totalAmount INTO v_total FROM Bill WHERE billId = v_billId;

  IF v_sumPaid >= v_total THEN
    UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = v_billId;
  ELSEIF v_sumPaid > 0 THEN
    UPDATE Bill SET paymentStatus = '部分支付', settledTime = NULL WHERE billId = v_billId;
  ELSE
    UPDATE Bill SET paymentStatus = '未支付', settledTime = NULL WHERE billId = v_billId;
  END IF;
END$$
DELIMITER ;

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
