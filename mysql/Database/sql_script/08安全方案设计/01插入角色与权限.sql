-- @author 巷北  
-- @time 2026-01-01 23:12:10  

-- 角色
INSERT IGNORE INTO AppRole(roleName, description) VALUES
('ADMIN', '系统管理员，最高权限'),
('MANAGER', '运营/店长，管理预订/入住/报表'),
('FRONTDESK', '前台，办理预订与入住/退房'),
('MAINTENANCE', '维修人员，管理维修记录'),
('FINANCE', '财务人员，处理账单/收款');

-- 权限
INSERT IGNORE INTO AppPermission(permName, description) VALUES
('RESERVATION.CREATE','创建预订'),
('RESERVATION.CANCEL','取消预订'),
('STAY.CHECKIN','办理入住'),
('STAY.CHECKOUT','办理退房'),
('ROOM.VIEW','查看房间信息'),
('ROOM.EDIT','修改房间状态/信息'),
('MAINTENANCE.MANAGE','管理维修记录'),
('BILL.VIEW','查看账单'),
('BILL.EDIT','修改账单'),
('PAYMENT.MANAGE','收款/退款'),
('CUSTOMER.MANAGE','管理客户资料'),
('REPORT.VIEW','查看运营/财务报表');

-- 角色-权限绑定
-- ADMIN 拥有所有权限
INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r CROSS JOIN AppPermission p
WHERE r.roleName = 'ADMIN';

-- MANAGER 权限
INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('RESERVATION.CREATE','RESERVATION.CANCEL','STAY.CHECKIN','STAY.CHECKOUT','ROOM.VIEW','ROOM.EDIT','BILL.VIEW','BILL.EDIT','REPORT.VIEW')
WHERE r.roleName = 'MANAGER';

-- FRONTDESK 权限
INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('RESERVATION.CREATE','RESERVATION.CANCEL','STAY.CHECKIN','STAY.CHECKOUT','ROOM.VIEW','CUSTOMER.MANAGE')
WHERE r.roleName = 'FRONTDESK';

-- MAINTENANCE 权限
INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('MAINTENANCE.MANAGE','ROOM.VIEW')
WHERE r.roleName = 'MAINTENANCE';

-- FINANCE 权限
INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('BILL.VIEW','BILL.EDIT','PAYMENT.MANAGE','REPORT.VIEW')
WHERE r.roleName = 'FINANCE';
