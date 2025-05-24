document.getElementById('paymentForm').addEventListener('submit', function(event) {
    event.preventDefault(); // Prevent form default submission

    // Collect form data
    const username = document.getElementById('username').value;
    const email = document.getElementById('email').value;
    const phone = document.getElementById('phone').value;
    const address = document.getElementById('address').value;
    const country = document.getElementById('country').value;
    const amount = document.getElementById('amount').value;
    const payment = document.getElementById('payment').value;

    // Build query string with encoded values
    const queryString = `?username=${encodeURIComponent(username)}&email=${encodeURIComponent(email)}&phone=${encodeURIComponent(phone)}&address=${encodeURIComponent(address)}&country=${encodeURIComponent(country)}&amount=${encodeURIComponent(amount)}&payment=${encodeURIComponent(payment)}`;

    // Redirect to the display page with query parameters
    window.location.href = 'after click submit btn.html' + queryString;
});

