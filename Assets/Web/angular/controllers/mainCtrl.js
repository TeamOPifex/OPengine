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
    }]);