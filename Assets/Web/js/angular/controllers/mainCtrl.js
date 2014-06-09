angular.module('opengine')
    .controller('mainCtrl', ['$scope', function ($scope) {
        $scope.title = 'OPifex Engine';

        $scope.websocket = new OPWebSocket();
        $scope.websocket.OnOpen = function() { SendColor(); $scope.$digest(); };
        $scope.websocket.OnClose = function() { $scope.$digest(); };

        $scope.red = 0;
        $scope.green = 0;
        $scope.blue = 0;

        function SendColor() {
            var builder = new BlobBuilder('color');
            builder.vec3($scope.red / 255.0, $scope.green / 255.0, $scope.blue / 255.0);
            $scope.websocket.Send(builder);
        };

        $scope.$watch('red', SendColor);
        $scope.$watch('green', SendColor);
        $scope.$watch('blue', SendColor);

        function SendFont() {
            var builder = new BlobBuilder('font');
            builder.vec2($scope.fontx, $scope.fonty);
            $scope.websocket.Send(builder);
        };

        $scope.fontx = 0;
        $scope.fonty = 0;
        $scope.$watch('fontx', SendFont);
        $scope.$watch('fonty', SendFont);


        $scope.websocket.OnMessage = function(reader) {
            var key = reader.str();
            switch(key) {
                case 'color3':
                    var color = reader.vec3();
                    $scope.red = color[0] * 255;
                    $scope.green = color[1] * 255;
                    $scope.blue = color[2] * 255;
                    $scope.$digest();
                break;
                case 'time':
                    $scope.time = reader.ui32();
                break;
            }
        }

        gyro.frequency = 20;
        //if(gyro.getFeatures().length > 0) {
            gyro.startTracking(function(o) {
                // o.x, o.y, o.z for accelerometer
                // o.alpha, o.beta, o.gamma for gyro
                $scope.fontx = o.x / 10.0;
                $scope.fonty = o.y / 10.0;
                $scope.$digest();
            });
        //}
    }]);