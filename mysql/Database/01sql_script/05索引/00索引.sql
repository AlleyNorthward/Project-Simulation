-- @author 巷北  
-- @time 2026-01-01 22:17:17  

-- 1) 加速按客户查最近入住 & 时间范围查询
ALTER TABLE Stay ADD INDEX idx_stay_customer_checkInTime (customerId, checkInTime);

-- 2) 加速按房型 + 时间段查询（用于判断预订与区间重叠）
ALTER TABLE Reservation ADD INDEX idx_reservation_roomtype_dates (roomTypeId, checkInDate, checkOutDate);

-- 3) 加速按房型统计、按房态筛选空闲房（JOIN RoomType + WHERE status）
ALTER TABLE Room ADD INDEX idx_room_roomtype_status (roomTypeId, status);

-- 4) 常按楼层筛选或分页查询房间时用
ALTER TABLE Room ADD INDEX idx_room_floor (floor);

-- 5) 加速按入住（checkInId）查账单与按支付状态筛选（账务报表）
ALTER TABLE Bill ADD INDEX idx_bill_checkIn_paymentStatus_createdTime (checkInId, paymentStatus, createdTime);

-- 6) 支付按账单/按时间检索 & 聚合（按月统计营收）
ALTER TABLE Payment ADD INDEX idx_payment_bill_paymentTime (billId, paymentTime);

-- 7) 汇总消费记录（按入住汇总、按时间段统计）
ALTER TABLE ConsumptionRecord ADD INDEX idx_consumption_checkin_time (checkInId, time);

-- 8) 查询某项服务被使用情况（按 serviceId + 时间）
ALTER TABLE ConsumptionRecord ADD INDEX idx_consumption_service_time (serviceId, time);

-- 9) 查询某商品被使用情况（按 productId + 时间）
ALTER TABLE ConsumptionRecord ADD INDEX idx_consumption_product_time (productId, time);

-- 10) 按会员等级、电话等快速检索客户（客户画像查询）
ALTER TABLE Customer ADD INDEX idx_customer_membership_phone (membershipLevelId, phone);

-- 11) 按部门/岗位检索员工（便于筛选组内成员与权限分配）
ALTER TABLE Employee ADD INDEX idx_employee_department_position (department, position);

-- 12) 维修查询：按房间查维修历史并按开始时间排序
ALTER TABLE Maintenance ADD INDEX idx_maintenance_room_startTime (roomId, startTime);

-- 13) 房型价格范围类查询（用于 basePrice BETWEEN / 排序）
ALTER TABLE RoomType ADD INDEX idx_roomtype_basePrice (basePrice);

-- 14) 账单按总额范围或排序的查询（如查 totalAmount BETWEEN）
ALTER TABLE Bill ADD INDEX idx_bill_totalAmount (totalAmount);












