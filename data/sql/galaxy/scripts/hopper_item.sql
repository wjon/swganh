
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `hopper_item` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `installation_id` bigint(20) DEFAULT NULL,
  `resource_id` bigint(20) DEFAULT NULL,
  `quantity` double NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_E6F7A6B9167B88B4` (`installation_id`),
  KEY `IDX_E6F7A6B989329D25` (`resource_id`),
  CONSTRAINT `FK_E6F7A6B9167B88B4` FOREIGN KEY (`installation_id`) REFERENCES `installation` (`id`),
  CONSTRAINT `FK_E6F7A6B989329D25` FOREIGN KEY (`resource_id`) REFERENCES `resource` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `hopper_item`;
    
/*!40000 ALTER TABLE `hopper_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `hopper_item` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
